from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect

# Create your views here.


def home(request):
    number = 1
    context = {'number':number}
    template = loader.get_template('home.html')
    return HttpResponse(template.render(context, request))

def conf(request):
    number= 3
    context = {'number':number}
    template = loader.get_template('config.html')
    return HttpResponse(template.render(context, request))

def dailyInput(request):
    number = 2
    context = {'number':number}
    template = loader.get_template('dailyInput.html')
    return HttpResponse(template.render(context, request))

def viewExpenses(request):
    number=1
    context = {'number' : number}
    template = loader.get_template('viewDetail.html')
    return HttpResponse(template.render(context, request))

def balanceSimulator(request):
    number = 4
    context ={ 'number':number}
    template = loader.get_template('simulator.html')
    return HttpResponse(template.render(context, request))


def savingHistory(request):
    number =5
    context = {'number':number}
    template = loader.get_template('savings.html')
    return HttpResponse(template.render(context, request))
