from core.views import download_temporary_file, showFirebaseJS, nivel
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import DataViewSet
from .views import LocationViewSet
from .views import FirmwareViewSet
from .views import DeviceViewSet
from .views import DeviceModelViewSet
from .views import DeviceLogViewSet
from .views import DataModelViewSet
app_name = "core"

router = DefaultRouter()
router.register(r'data', DataViewSet, basename='data')
router.register(r'location', LocationViewSet, basename='location')
router.register(r'firmware', FirmwareViewSet, basename='firmware')
router.register(r'device', DeviceViewSet, basename='device')
router.register(r'device_model', DeviceModelViewSet, basename='device_model')
router.register(r'device_log', DeviceLogViewSet, basename='device_log')
router.register(r'data_model', DataModelViewSet, basename='data_model')

urlpatterns = [
    path('', include(router.urls)),
    path("download/<slug:uuid>/", download_temporary_file, name="download-temporary-file"),
    path("firebase-messaging-sw.js", showFirebaseJS, name="show-firebase-ja"),
    path('home/', nivel, name='home'),
]