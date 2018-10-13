from django.shortcuts import render, redirect
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect
from .models import Concept
from django.contrib.auth.models import User
from .models import Concept, DailyInput
from .forms import ConfigurationForm, DailyInputForm
import json
import datetime
from datetime import datetime as dt
from django.contrib.auth import views as auth_views

# Create your views here.

def deleteDailyInput(request):
    daily = DailyInput.objects.filter(id= request.GET['id_concept'], date_from__gte =dt.strptime(str(request.GET['date']), "%d/%m/%Y"))
    if(len(daily) > 0):
        daily[0].delete();
    data ={}
    return HttpResponse(json.dumps(data), content_type="application/json")

def getConcepts(id_usuario):
    conceptos = Concept.objects.filter(id_user=id_usuario, is_disabled=False)
    return conceptos

def home(request):
    number = 1
    context = {'number':number}
    template = loader.get_template('home.html')
    if request.user.is_authenticated:

        return HttpResponse(template.render(context, request))
    else:
        return redirect('login/')

def conf(request):
    current_id_user = request.user.id
    number= 3
    form = ConfigurationForm()
    conceptos = getConcepts(current_id_user)

    template = loader.get_template('config.html')
    message = ""
    if request.method == "POST":
        form = ConfigurationForm(request.POST)

        if form.is_valid():
            print("form is valid")
            if(form.cleaned_data['isNewConcept'] == -1):
                message = "NEW CONCEPT CREATED"
                newConcept = Concept(name = form.cleaned_data['name'], value = form.cleaned_data['value'], period = form.cleaned_data['period'], type=form.cleaned_data['isExpense'], id_user= current_id_user)
                newConcept.save()
            else:
                message = "CONCEPT MODIFIED"
                concept = Concept.objects.filter(id_user=current_id_user, id=form.cleaned_data['isNewConcept'])[0]
                concept.name = form.cleaned_data['name']
                concept.value = form.cleaned_data['value']
                concept.period = form.cleaned_data['period']
                concept.type= form.cleaned_data['isExpense']
                concept.save()
        else:
            print("form invalid")
            print(form.errors)
    context = {'number':number, 'conceptos': conceptos, 'form': form, 'message':message}
    return HttpResponse(template.render(context, request))

def deleteConcept(request):
    current_id_user = request.user.id
    concept = Concept.objects.filter(id_user =current_id_user, id= request.GET['id_concept'])[0]
    concept.is_disabled = True
    concept.save()
    data = {}
    return HttpResponse(json.dumps(data), content_type="application/json")

def dailyInput(request):
    current_id_user = request.user.id
    conceptos = getConcepts(current_id_user)
    number = 2
    isDailyInput = False
    todayDate = datetime.datetime.today().strftime('%d/%m/%Y')
    message =""
    print(request.method)
    if request.method == 'POST':
        form  = DailyInputForm(request.POST)
        isDailyInput = True
        if form.is_valid():
            message = "daily input added"
            dailyInput = DailyInput(id_user=current_id_user, id_concept=form.cleaned_data['id_concepto'], value=form.cleaned_data['value'], date_from = form.cleaned_data['from_date'])
            dailyInput.save()
        else:
            message = "couldn't save this daily input"

    form = DailyInputForm()
    template = loader.get_template('dailyInput.html')
    context = {'number':number, 'conceptos': conceptos, 'todayDate':todayDate,
                'isDailyInput': isDailyInput, 'form': form, 'message':message}
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
    number = 5
    context = {'number':number}
    template = loader.get_template('savings.html')
    return HttpResponse(template.render(context, request))
