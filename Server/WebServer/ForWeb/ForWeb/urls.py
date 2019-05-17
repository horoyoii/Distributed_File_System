"""ForWeb URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from mainapp.views import *
from mainapp import views
from django.conf.urls import url, include

from rest_framework.authtoken.views import obtain_auth_token  

from rest_framework import routers

router =routers.DefaultRouter()
#router.register(r'fileinfos', views.FileInfoViewSet)


"""
REST는 WEB 관련 기술이다.
데이터를 주고받는 일종의 규약이다.
 - 이 규약을 통한 이점 
  1) 하나의 서버로 여러 종류의 클라이언트(web, ios, android)에 대응하도록 할 떄


HTTP의 기본 메서드(GET, POST, PUT, DELETE)를 통해 유지한다.

URI의 예 [ Uniform Resource Identifier ] : 인터넷 상의 자원 식별자
http://www.remotty.com/countries/korea


- CRUD를 URI에 표현하면 안된다. Rest에서 
 GET /posts/13/delete HTTP/1.1
 POST /posts/write HTTP/1.1
 GET /delete?id=55 HTTP/1.1


"""

#  GET : /users/{userid}/devices (일반적으로 소유 ‘has’의 관계를 표현할 때)
"""
collection : uri에 표시할 때 복수형으로 표현
document : 단수형으로 표현

http:// restapi.example.com/sports/soccer/players/13
>> sports, players : collection
"""
urlpatterns = [
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework')),
    path('admin/', admin.site.urls),
    path('index/', index),

    url(r'^api/v2/users/(?P<userid>[\w\-]+)/filelists/$',views.FileList.as_view()),
    url(r'^api/v2/users/(?P<userid>[\w\-]+)/filelists/(?P<input_fid>[0-9]+)/$',views.FileListDetail.as_view()),
    
    path('hello', views.HelloView.as_view(), name='hello'),
    path('obtain_token', views.Login.as_view(), name='login'),
    

]
