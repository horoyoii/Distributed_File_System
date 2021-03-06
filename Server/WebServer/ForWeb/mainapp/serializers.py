from mainapp.models import Fileinfo
from mainapp.models import Filelists
from mainapp.models import User
from rest_framework import serializers


# 직렬화의 이유
# 다른 환경 간에 데이터를 주고받기 위해서는 동일한 데이터 구조를 가져야 하기 때문이다.

class FileinfoSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Fileinfo
        fields = ('uid','name', 'lastupdatetime')

class FileListSerializer(serializers.ModelSerializer):
    class Meta:
        model = Filelists
        fields = ('fid', 'name', 'lastupdatetime', 'uid')

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ('id', 'pw')
