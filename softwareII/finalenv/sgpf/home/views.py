from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect
from .models import Concept
from django.contrib.auth.models import User
# Create your views here.

def getConcepts(id_usuario):
    conceptos = Concept.objects.filter(id_user=id_usuario)
    return conceptos

def home(request):
    number = 1
    context = {'number':number}
    template = loader.get_template('home.html')
    return HttpResponse(template.render(context, request))

def conf(request):
    #if request.method == "POST":

    current_id_user = request.user.id
    number= 3
    conceptos = getConcepts(current_id_user)
    context = {'number':number, 'conceptos': conceptos}
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
