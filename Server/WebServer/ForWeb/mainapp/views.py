from django.shortcuts import render
from .models import Fileinfo as FileInfoModel

from .models import Filelists as FileListsModel
from mainapp.serializers import FileListSerializer

from .models import User as UserModel

from rest_framework import viewsets
from mainapp.serializers import FileinfoSerializer

from mainapp.serializers import UserSerializer

from django.http import Http404
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status

from django.http import HttpResponse,HttpResponseNotFound

import jwt,json
from django.conf import settings

from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import get_authorization_header, BaseAuthentication
from rest_framework.decorators import api_view, permission_classes, authentication_classes
from rest_framework_jwt.serializers import jwt_payload_handler
from rest_framework_jwt.authentication import JSONWebTokenAuthentication

from rest_framework.permissions import AllowAny
from django.contrib.auth.signals import user_logged_in


from mainapp.authentificate import CustomAuthentificate

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

class FileList(APIView):
    
    @CustomAuthentificate
    def get(self, request, contents, format=None):
        print("====caleld get in FileList====")
    
        userid = contents['userid']

        result =UserModel.objects.get(id=userid)
        fileinfo = FileListsModel.objects.filter(uid=result.uid)

        serializer_class = FileListSerializer(fileinfo, many=True)
        return Response(serializer_class.data)
    

    def post(self, request, userid, format=None):
        serializer = FileListSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)




class FileListDetail(APIView):

    def get_object(self, userid, input_fid):
        try:
            result =UserModel.objects.get(id=userid)
            fileinfo = FileListsModel.objects.filter(uid=result.uid).filter(fid = input_fid)
            return fileinfo
        except UserModel.DoesNotExist:
            raise Http404

    @CustomAuthentificate
    def get(self, request, contents, format=None): # contents 는 데코레이터에서 kwargs로 넘어온다.
        print("====calleld get in FileListDetail====")
        userid = contents['userid']
        input_fid = contents['input_fid']

        #fileListDetailItem = self.get_object(userid, input_fid)
        
        try:
            result =UserModel.objects.get(id=userid)
            fileListDetailItem = FileListsModel.objects.filter(uid=result.uid).filter(fid = input_fid)
        except UserModel.DoesNotExist:
            raise Http404

        
        serializer_class = FileListSerializer(fileListDetailItem, many=True)
        return Response(serializer_class.data)



    """
    def put(self, request, userid, input_fid, format=None):
        fileListDetailItem = self.get_object(userid, input_fid)
        serializer = FileListSerializer(fileListDetailItem, data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    """


    def delete(self, request, userid, input_fid, format=None):
        fileListDetailItem = self.get_object(userid, input_fid)
        fileListDetailItem.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)

# 관련 용례나 예제를 찾을려고만 헀지 직접 구현하려고 생각하지 못하였다.
# 여섯 번의 도전끝에 request header의 token을 직접 파싱하여 decoding하는, 프로시져를 직접 구현하여 문제를 해결할 수 있었다.


class HelloView(APIView):

    @CustomAuthentificate
    def get(self, request, user):
        print("hello called")
        content = {'message': 'Hello, World!'+user.id}
        return Response(content)



# 인증 ======================================================================================================

# JWT 적용 사이트 
# https://medium.com/python-pandemonium/json-web-token-based-authentication-in-django-b6dcfa42a332
# https://code.tutsplus.com/tutorials/how-to-authenticate-with-jwt-in-django--cms-30460


class Login(APIView):

    def post(self, request, *args, **kwargs):
        if not request.data:
            return Response({'Error': "Please provide username/password"}, status="400")
    
        username = request.data['username']
        password = request.data.get('password','')
        

        try:
            user = UserModel.objects.get(id=username, pw=password)

        except UserModel.DoesNotExist:
            return Response({'Error': "Invalid username/password"}, status="400")
        
        # JWT : head - payload - signature
        if user:
            payload = {
                'id': user.id,
                'uid': user.uid,    
                'scope' : ['can read', 'can delete']
            }

            jwt_token = {'token': jwt.encode(payload, settings.SECRET_KEY)} # SECRET_KEY가 "ABCDE"다
            print(jwt_token)
            
            return Response(
              jwt_token,
              status=200,
              content_type="application/json"
            )

        else:
            return Response(
              json.dumps({'Error': "Invalid credentials"}),
              status=400,
              content_type="application/json"
            )


import mimetypes
import os
"""
def get_downlaod(request):
    #file_full_path = "/tmp/{0}".format(filename)
    file_full_path = "C:\\Users\\user\\Desktop\\123.PNG"
    print(file_full_path)

    with open(file_full_path,'r') as f:
        data = f.read()
    print(data)

    response = HttpResponse(data, content_type=mimetypes.guess_type(file_full_path)[0])
    response['Content-Disposition'] = "attachment; filename={0}".format("hey")
    response['Content-Length'] = os.path.getsize(file_full_path)
    return response
    #return HttpResponse("HI?")
"""

import mimetypes

def get_downlaod(request, userid, filename):
    ...

    file_location = 'D:\\Sources\\SaaS\\Server\\WebServer\\ForWeb\\mainapp\\files\\aaa.mp4'
    #print(mimetypes.guess_type(file_location))
    print(userid)
    print(filename)


     # 'read byte mode인 경우 text, PNG, mp4 전송 가능
    try:    
        with open(file_location, 'rb') as f:
           file_data = f.read()

        # sending response 
        response = HttpResponse(file_data, content_type=mimetypes.guess_type(file_location))
        response['Content-Disposition'] = 'attachment; filename="aaa.mp4"'

    except IOError:
        # handle file not exist case here
        response = HttpResponseNotFound('<h1>File not exist</h1>')

    return response

    # 'read byte mode인 경우 text, PNG 전송 가능
    """
    try:
        print("ok")
        with open(file_location, "rb") as f:
            return HttpResponse(f.read(), content_type=mimetypes.guess_type(file_location))
    except IOError:
        print("called")
        red = Image.new('RGBA', (1, 1), (255,0,0,0))
        response = HttpResponse(content_type="image/PNG")
        red.save(response, "PNG")
        return response
    """    
    