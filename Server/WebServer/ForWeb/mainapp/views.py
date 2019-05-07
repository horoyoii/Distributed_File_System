from django.shortcuts import render
from .models import Fileinfo as FileInfoModel
# Create your views here.
def index(request):
    fileinfoList = FileInfoModel.objects.all()

    return render(request, 'index.html', {'fileinfoList':fileinfoList})
