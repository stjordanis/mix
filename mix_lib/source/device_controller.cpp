#include <mix/device_controller.h>
#include <mix/io_device.h>

#include "internal/helpers.hpp"

#include <map>

#include <cassert>

using namespace mix;

namespace {

struct DeviceProxy final :
	public IIODevice
{
	DeviceProxy(IIODeviceListener& listener,
		DeviceId id,
		std::unique_ptr<IIODevice> device)
			: listener_{listener}
			, device_{std::move(device)}
			, id_{id}
	{
	}

	virtual DeviceId id() const override
	{
		// Note: we are not calling `device_->id()` since
		// it's not make sense if Device will chnage its ID
		return id_;
	}

	virtual IODeviceType type() const
	{
		// #TODO: cache type also as with `id()`
		return device_->type();
	}

	virtual bool ready() const override
	{
		return device_->ready();
	}

	virtual int block_size() const override
	{
		return device_->block_size();
	}

	virtual Word read_next(DeviceBlockId block_id) override
	{
		listener_.on_device_read(id_, block_id);
		return device_->read_next(block_id);
	}

	virtual void write_next(DeviceBlockId block_id, const Word& word) override
	{
		listener_.on_device_write(id_, block_id);
		device_->write_next(block_id, word);
	}

private:
	IIODeviceListener& listener_;
	std::unique_ptr<IIODevice> device_;
	DeviceId id_;
};

} // namespace

struct DeviceController::Impl final :
	public IIODeviceListener
{
	IIODeviceListener* listener_;
	std::map<DeviceId, DeviceProxy> devices_;

	Impl(IIODeviceListener* listener)
		: listener_{listener}
		, devices_{}
	{
	}

	IIODevice& device(DeviceId id)
	{
		return devices_.at(id);
	}

	void inject_device(std::unique_ptr<IIODevice> device)
	{
		const auto id = device->id();
		devices_.erase(id);
		(void)devices_.emplace(std::piecewise_construct,
			std::make_tuple(id),
			std::forward_as_tuple(*this, id, std::move(device)));
	}

	virtual void on_device_read(DeviceId id, DeviceBlockId block_id) override
	{
		internal::InvokeListener(listener_, &IIODeviceListener::on_device_read,
			id, block_id);
	}

	virtual void on_device_write(DeviceId id, DeviceBlockId block_id) override
	{
		internal::InvokeListener(listener_, &IIODeviceListener::on_device_write,
			id, block_id);
	}
};

DeviceController::DeviceController(IIODeviceListener* listener /*= nullptr*/)
	: impl_{std::make_unique<Impl>(listener)}
{
}

DeviceController::~DeviceController() = default;

void DeviceController::set_listener(IIODeviceListener* listener)
{
	impl_->listener_ = listener;
}

IIODevice& DeviceController::device(DeviceId id)
{
	return impl_->device(id);
}

void DeviceController::inject_device(std::unique_ptr<IIODevice> device)
{
	assert(device);
	impl_->inject_device(std::move(device));
}
