from django.shortcuts import render
from .models import Fileinfo as FileInfoModel
from .models import User as UserModel

from rest_framework import viewsets
from mainapp.serializers import FileinfoSerializer


from django.http import Http404
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status

# Create your views here.
def index(request):
    fileinfoList = FileInfoModel.objects.all()

    return render(request, 'index.html', {'fileinfoList':fileinfoList})




"""
뷰는 함수 혹은 클래스로 작성할 수 있다.
공통점은 요청을 받아서 응답을 반환하는 객체라는 것이다.

여러가지 방법들 설명 사이트
http://raccoonyy.github.io/drf3-tutorial-3/

"""

# 클래스 기반 뷰
class FileInfoList(APIView):
    def get(self, request, formnat=None):
        print("caleld get in FileinfoList")
        fileinfo = FileInfoModel.objects.all()
        serializer_class = FileinfoSerializer(fileinfo, many=True)
        return Response(serializer_class.data)

    # post put delete 추가하기



class FileInfoDetail(APIView):
    def get_object(self, uid):
        try:
            return FileInfoModel.objects.filter(uid=uid)
        except FileInfoModel.DoesNotExist:
            return Http404

    def get(self, request, uid, format=None):
        print("caleld get in FileinfoDetail")
        fileinfo = self.get_object(uid)
        serializer_class = FileinfoSerializer(fileinfo, many=True)
        return Response(serializer_class.data)
    


class FileList(APIView):
    def get(self, request, userid, format=None):
        print("====caleld get in FileList====")

        ## objects를 대상으로 get을 사용한다.
        result =UserModel.objects.get(id=userid)

        fileinfo = FileInfoModel.objects.filter(uid=result.uid)
        serializer_class = FileinfoSerializer(fileinfo, many=True)
        return Response(serializer_class.data)

class FileDetail(APIView):
    def get_object(self, pk):
        try:
            return FileInfoModel.objects.get(pk=pk)
        except:
            raise Http404
    


    







# JSON으로 데이터를 보내야 하는 Restful API
# Django ORM의 queryset은 HTML로 렌더링 되는 Django template을 사용하지 않고
# Qeuryset을 Nested한 JSON으로 매핑되는 과정을 거쳐야 하는데 이것을 Serializer가 수행한다.

"""
class FileInfoViewSet(viewsets.ModelViewSet):
    queryset = FileInfoModel.objects.all()

    serializer_class = FileinfoSerializer
"""



    