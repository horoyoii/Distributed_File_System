"""
created by Horoyoii on 2019.05.17

Request Header에서 JWT를 파싱하여 decoding하는 작업을 수행하는 모듈


"""


from django.conf import settings

from rest_framework.authentication import get_authorization_header
from django.http import HttpResponse

from mainapp.models import User
import jwt,json
from rest_framework.response import Response


class CustomAuthentificate:
    def __init__(self, f):
        print("__init__ called")
        self.func = f
    
    # url 에 포함된 parameter는 kwargsㅇ ㅔ포함된다.
    def __call__(self, request, *args, **kwargs):
        print("called __call__")
        """
        # 권한 : can read can delete 등등
        # expired : 등등 ... 
        """
        rtn_result = self.authenticate(request)
        print(rtn_result)
        if isinstance(rtn_result, tuple) == False:
            return Response({'Error': "Token is invalid"}, status="403") # error 만 출력된다. 클라이언트에서
        
        

        print("==============")
        return self.func(self, *args, request, kwargs)



    def authenticate(self, request):
        print("==========auto")
        auth = get_authorization_header(request).split()
        if not auth or auth[0].lower() != b'token':
            print("none")
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
        #model = self.get_model()
       

        try:
            payload = jwt.decode(token, settings.SECRET_KEY)
            id = payload['id']
            uid = payload['uid']
            scope = payload['scope']
            msg = {'Error': "Token mismatch",'status' :"401"}     
            
            user = User.objects.get(
                uid=uid,
                id=id,
                #is_active=True
            )
            
            # 유저 모델에 token 필드도 추가해야함
            """
            if not user.token['token'] == token:
                raise exceptions.AuthenticationFailed(msg)
            """

        except jwt.ExpiredSignature or jwt.DecodeError or jwt.InvalidTokenError or jwt.exceptions.InvalidSignatureError: 
            return HttpResponse({'Error': "Token is invalid"}, status="403")
        except User.DoesNotExist:
            return HttpResponse({'Error': "Internal server error"}, status="500")

        return (user, token)

    def authenticate_header(self, request):
        return 'Token'