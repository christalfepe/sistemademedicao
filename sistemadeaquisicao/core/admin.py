from django.contrib import admin
from .models import Data, DeviceModel, Device, Location, Firmware, DataModel, DeviceLog

admin.site.register(Data)
admin.site.register(Device)
admin.site.register(DataModel)
admin.site.register(DeviceModel)
admin.site.register(Location)
admin.site.register(Firmware)
admin.site.register(DeviceLog)