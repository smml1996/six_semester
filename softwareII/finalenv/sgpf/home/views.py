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
from login.models import Savings,Savings_Percentage
from django.core import serializers
from django.db import connection
from django.core.serializers.json import DjangoJSONEncoder

# Create your views here.

def DeleteDailyInput(request):
    # this is a get request
    sp = Savings_Percentage.objects.filter(id_user=request.user.id)
    sp[0].value = request.GET['value']
    sp.save()
    data ={} # use empty json as response meaning end of processing
    return HttpResponse(json.dumps(data), content_type="application/json")

def changeSavingsPercentage(request):
    daily = DailyInput.objects.filter(id= request.GET['id_concept'], date_from__gte =dt.strptime(str(request.GET['date']), "%d/%m/%Y"))


def getConcepts(id_usuario):
    # get only concepts which are not disabled by user
    conceptos = Concept.objects.filter(id_user=id_usuario, is_disabled=False)
    return conceptos

def home(request):
    number = 1 # variable sent to interface, used to highlight home tab in navbar
    template = loader.get_template('home.html')
    if request.user.is_authenticated:
        todayDate = datetime.datetime.today().strftime('%d/%m/%Y')
        print(request.user.id)
        currentSaving = Savings.objects.filter(id_user=request.user.id)[0].value
        context = {'number':number,'currentSaving':currentSaving, 'todayDate': todayDate }
        return HttpResponse(template.render(context, request))
    else:
        # if user not authenticated send her/him to login page
        return redirect('login/')

def SaveConcept(request):
    number= 3 # variable sent to interface, used to highlight home tab in navbar
    current_id_user = request.user.id
    #getting the desired saving percentage of current user
    currentPercentage = Savings_Percentage.objects.filter(id_user=current_id_user)[0].percentage
    currentPercentage = currentPercentage*100 # getting the percentage value in %
    form = ConfigurationForm()
    conceptos = getConcepts(current_id_user)

    template = loader.get_template('config.html')
    message = ""
    if request.method == "POST":
        form = ConfigurationForm(request.POST)
        # (save concept) case
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
    context = {'number':number, 'conceptos': conceptos, 'form': form, 'message':message, 'currentPercentage':currentPercentage}
    return HttpResponse(template.render(context, request))

def disableConcept(request):
    # function used for ajax
    current_id_user = request.user.id
    concept = Concept.objects.filter(id_user =current_id_user, id= request.GET['id_concept'])[0]
            # get concept of requsted user with specific id
            # delete first one
    concept.is_disabled = True
    concept.save()
    data = {} #send empty data as json, meaning end of processing
    return HttpResponse(json.dumps(data), content_type="application/json")

def AddDailyInput(request):
    current_id_user = request.user.id
    conceptos = getConcepts(current_id_user) # variable meant to be sent to interface
    number = 2 # variable sent to interface, used to highlight home tab in navbar
    isDailyInput = False # we dont have any message yet to show to user
    todayDate = datetime.datetime.today().strftime('%d/%m/%Y')
                #default date to be displayed in this interface, is today's date
    message =""
    print(request.method)
    if request.method == 'POST':
        form  = DailyInputForm(request.POST)
        isDailyInput = True # we have some message to show to user
        if form.is_valid():
            message = "daily input added"
            # creating new dailyInput:  (Add daily input)
            dailyInput = DailyInput(id_user=current_id_user, id_concept=form.cleaned_data['id_concepto'], value=form.cleaned_data['value'], date_from = form.cleaned_data['from_date'])
            dailyInput.save()
        else:
            #form is not valid, dont have all data to create new daily input
            message = "couldn't save this daily input"

    form = DailyInputForm()
    template = loader.get_template('dailyInput.html')
    context = {'number':number, 'conceptos': conceptos, 'todayDate':todayDate,
                'isDailyInput': isDailyInput, 'form': form, 'message':message}
    return HttpResponse(template.render(context, request))

def visualize(request):
    type = request.GET['type']
    from_date = request.GET['from']
    to_date = request.GET['to']
    print(type == '1')

    with connection.cursor() as cursor:
        #if type == '1':
            # user is trying to visualize her/his incomes
        cursor.execute("SELECT di.value, period, name, date_from FROM home_DailyInput AS di JOIN home_Concept AS c ON c.id=di.id_concept WHERE di.id_user=15 AND ((date_from<= %s AND period!=0) )  AND type=true ORDER BY di.date_from DESC",)
        #else:
            #user is trying to visualize her/his expenses
        #    cursor.execute("SELECT di.value, period, name, date_from FROM home_DailyInput AS di JOIN home_Concept AS c ON c.id=di.id_concept WHERE di.id_user=15 AND ((date_from<= %s AND period!=0) OR (date_from>=%s AND date_from<=%s and period=0)) AND type=false ORDER BY di.date_from DESC",[to_date])
        json_data = json.dumps(cursor.fetchall(), sort_keys=True, indent=4, separators=(',', ': '), cls=DjangoJSONEncoder)
        return HttpResponse(json_data, content_type="application/json")

def simulateBalance(request):
    number = 4 # variable sent to interface, used to highlight home tab in navbar
    context ={ 'number':number}
    template = loader.get_template('simulator.html')
    return HttpResponse(template.render(context, request))


def visualizeSavings(request):
    number = 5 # variable sent to interface, used to highlight home tab in navbar
    context = {'number':number}
    template = loader.get_template('savings.html')
    return HttpResponse(template.render(context, request))
