from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect

# Create your views here.


def home(request):
    context = {}
    template = loader.get_template('home.html')
    return HttpResponse(template.render(context, request))

def conf(request):
    context = {}
    template = loader.get_template('config.html')
    return HttpResponse(template.render(context, request))

def dailyInput(request):
    context = {}
    template = loader.get_template('dailyInput.html')
    return HttpResponse(template.render(context, request))
