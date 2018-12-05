from django.shortcuts import render, redirect
from django.template import loader
from django.http import HttpResponse,Http404,HttpResponseRedirect
from django.contrib.auth import login, authenticate
from django.contrib.auth.models import User
from login.forms import SignUpForm
from judge.models import Scores


def signup(request):
    #use case: Sign Up
    message = "" # help messages for user
    if request.method == 'POST':
        form = SignUpForm(request.POST) # get form filled by user
        if form.is_valid(): # django validations
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            email = form.cleaned_data.get('email')
            form.save()
            user = authenticate(username=username, password=raw_password)
            score  = Scores(user= user, problem1_status="not solved", problem2_status="not solved", problem3_status="not solved", score=0.00)
            score.save()
            login(request, user)
            return redirect('/')
        else:
            print("form not valid")
            message = form.errors # gather error messages to show to user
    form = SignUpForm()
    return render(request, 'registration/signup.html',{'form': form, 'message': message})
