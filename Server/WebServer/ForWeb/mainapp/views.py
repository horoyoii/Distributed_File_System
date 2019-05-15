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

from django.http import HttpResponse

import jwt,json
from django.conf import settings

from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import get_authorization_header, BaseAuthentication
from rest_framework.decorators import api_view, permission_classes, authentication_classes
from rest_framework_jwt.serializers import jwt_payload_handler
from rest_framework_jwt.authentication import JSONWebTokenAuthentication

from rest_framework.permissions import AllowAny
from django.contrib.auth.signals import user_logged_in

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
    


# Real ======================================================================================

class FileList(APIView):
    def get(self, request, userid, format=None):
        print("====caleld get in FileList====")

        ## objects를 대상으로 get을 사용한다.
        result =UserModel.objects.get(id=userid)
        print(result.uid)
        fileinfo = FileListsModel.objects.filter(uid=result.uid)
        print(fileinfo)


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


    def get(self, request, userid, input_fid, format=None):
        print("====calleld get in FileListDetail====")

        fileListDetailItem = self.get_object(userid, input_fid)
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
    def get(self, request):
        print("hello called")
        
        # 이부분을 데코레이터로 직접 만들어버린다. 이제
        token = request.META.get('HTTP_AUTHORIZATION')
        print(request.META.get('HTTP_AUTHORIZATION'))
        print(token[6:])

        payload = jwt.decode(token[6:], settings.SECRET_KEY)
        print(payload)
        id = payload['id']
        uid = payload['uid']

        content = {'message': 'Hello, World!'+id}
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



class TokenAuthentication(BaseAuthentication):

    model = None

    def get_model(self):
        return User

    def authenticate(self, request):
        print("==========auto")
        auth = get_authorization_header(request).split()
        if not auth or auth[0].lower() != b'token':
            return None

        if len(auth) == 1:
            msg = 'Invalid token header. No credentials provided.'
            raise exceptions.AuthenticationFailed(msg)
        elif len(auth) > 2:
            msg = 'Invalid token header'
            raise exceptions.AuthenticationFailed(msg)

        try:
            token = auth[1]
            if token=="null":
                msg = 'Null token not allowed'
                raise exceptions.AuthenticationFailed(msg)
        except UnicodeError:
            msg = 'Invalid token header. Token string should not contain invalid characters.'
            raise exceptions.AuthenticationFailed(msg)

        return self.authenticate_credentials(token)

    def authenticate_credentials(self, token):
        print("==========auto")
        model = self.get_model()
        payload = jwt.decode(token, "SECRET_KEY")
        email = payload['email']
        userid = payload['id']
        msg = {'Error': "Token mismatch",'status' :"401"}
        try:
            
            user = User.objects.get(
                email=email,
                id=userid,
                is_active=True
            )
            
            if not user.token['token'] == token:
                raise exceptions.AuthenticationFailed(msg)
               
        except jwt.ExpiredSignature or jwt.DecodeError or jwt.InvalidTokenError:
            return HttpResponse({'Error': "Token is invalid"}, status="403")
        except User.DoesNotExist:
            return HttpResponse({'Error': "Internal server error"}, status="500")

        return (user, token)

    def authenticate_header(self, request):
        return 'Token'









    
    






    