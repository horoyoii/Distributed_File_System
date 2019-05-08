from django.shortcuts import render
from .models import Fileinfo as FileInfoModel

from rest_framework import viewsets
from mainapp.serializers import FileinfoSerializer

# Create your views here.
def index(request):
    fileinfoList = FileInfoModel.objects.all()

    return render(request, 'index.html', {'fileinfoList':fileinfoList})





class FileInfoViewSet(viewsets.ModelViewSet):
    queryset = FileInfoModel.objects.all()
    serializer_class = FileinfoSerializer

    