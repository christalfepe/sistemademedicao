from rest_framework import serializers
from .models.data import Data
from .models.location import Location
from .models.firmware import Firmware
from .models.device import Device
from .models.device_model import DeviceModel
from .models.device_log import DeviceLog
from .models.data_model import DataModel

class DataSerializer(serializers.ModelSerializer):
    class Meta:
        model = Data
        fields = ['log', 'model', 'value']

class LocationSerializer(serializers.ModelSerializer):
    class Meta:
        model = Location
        fields = ['name', 'description', 'address', 'latitude', 'longitude']

class FirmwareSerializer(serializers.ModelSerializer):
    class Meta:
        model = Firmware
        fields = ['id', 'file', 'version', 'detail', 'model', 'created_at']

class DeviceSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = ['id', 'mac', 'model', 'last_call', 'location', 'latitude', 'longitude']
    
class DeviceModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = DeviceModel
        fields = ['id', 'name']

class DeviceLogSerializer(serializers.ModelSerializer):
    class Meta:
        model = DeviceLog
        fields = ['id', 'device']
    
class DataModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = DataModel
        fields = ['id', 'name', 'description', 'model', 'reference_tag', 'alert_per_time', 'warning_alert', 'danger_alert']