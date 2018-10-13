from django.shortcuts import render, redirect
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect
from django.contrib.auth import login, authenticate
from django.contrib.auth.models import User

from login.forms import SignUpForm


# Create your views here.
def isEmailValid(email):
    users = User.objects.all()
    temp = users.filter(email=email)
    return len(temp) == 0

def signUpRequest(request):
    message = ""
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            email = form.cleaned_data.get('email')
            if isEmailValid(email):
                form.save()
                return redirect('../login/')
            else:
                message = "not valid email"
        else:
            print("form not valid")
            message = form.errors
    form = SignUpForm()
    return render(request, 'registration/registro.html',{'form': form, 'message': message})


def index(request):
    context = {}
    template = loader.get_template('login.html')
    return HttpResponse(template.render(context, request))
