# Distributed_File_System
# 1. What is It? 
  데스크탑의 특정 디렉토리에서 파일의 추가, 수정 등이 발생 시 자동으로 서버에 동기화 업로드를 수행하는 클라우드 시스템
  
# 2. Components

### 1) 데스크탑 서비스   
백그라운드에서 동작하며 특정 디렉토리를 추적하여 이벤트 발생 시 서버로 파일 업로드 (Boost Asio C++)
### 2) 서버 
##### 2-1) Boost 서버 : 데스크탑 클라이언트로부터 파일을 전송받아 MySQL에 저장. (Boost Asio C++)
##### 2-2) Django서버 : 안드로이드 앱에 JWT 토큰 인증 기반하여 관련 정보에 대한 REST API 제공 / 웹 서버 (Django REST Framework)

### 3) 안드로이드 어플 
Django 서버에 대한 프론트엔드 


  
  



![image](https://user-images.githubusercontent.com/34915108/57904559-2d3b1480-78ae-11e9-97b7-3a26ac80bf9b.png)

