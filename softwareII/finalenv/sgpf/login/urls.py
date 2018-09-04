from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^$', views.signUpRequest, name='signup')
	#url(r'^$',views.index, name="index"),

]
