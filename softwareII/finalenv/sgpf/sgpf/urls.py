"""sgpf URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import include, path
from django.contrib.auth import views as auth_views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('home/', include('home.urls')),
    #path(r'', include('login.urls'), name="login"),
    path('', auth_views.LoginView.as_view(template_name='registration/login.html')),
    path('logout/', include('django.contrib.auth.urls')),
    path('signup/', include('login.urls')),

    #path(r'', include('django.contrib.auth.urls'), name="login"),
]
