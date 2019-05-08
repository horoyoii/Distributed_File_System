from mainapp.models import Fileinfo
from rest_framework import serializers


class FileinfoSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Fileinfo
        fields = ('uid','name', 'lastupdatetime')