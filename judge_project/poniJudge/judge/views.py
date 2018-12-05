from django.shortcuts import render
from django.template import loader
from django.shortcuts import redirect
from django.http import HttpResponse,Http404,HttpResponseRedirect
from .models import Scores
from .forms import UploadSolutionForm

URL_RUN = 'https://api.hackerearth.com/v3/code/run/'
URL_COMPILE = 'https://api.hackerearth.com/v3/code/compile/'
# Create your views here.
def index(request):
    template = loader.get_template('index.html')
    context = {}
    return HttpResponse(template.render(context, request))

def judgeCodigo(request, value):
    pass

def problemView(request, value):
    if request.method == "POST":
        form = UploadSolutionForm(request.POST, request.FILES)
        if form.is_valid():
            codigo = (request.FILES['solution']).read()
            judgeCodigo(codigo, value)
            return redirect('../../leaderboard/')

    template = loader.get_template('problem.html')
    title = ''
    time_limit = 1
    memory_limit = 256
    form = UploadSolutionForm()
    if value == '1':
        title = 'Theatre Square'
    elif value == '2':
        title = 'Vasya and Multisets'
    else:
        title= 'Relatively Prime Graph'
        time_limit = 2

    context = {'value':value, 'title':title, 'time_limit':time_limit, 'memory_limit':memory_limit, 'form':form, 'message':message}
    return HttpResponse(template.render(context, request))

def Leaderboard(request):
    template = loader.get_template('leaderboard.html')
    scores = Scores.objects.all().order_by('-score').values('user__username', 'problem1_status', 'problem2_status', 'problem3_status', 'score')
    context = {'scores':scores}
    return HttpResponse(template.render(context, request))
