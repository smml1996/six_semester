from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^conf/$', views.conf, name='conf'),
	url(r'^daily_input/$', views.dailyInput, name='dailyInput'),
	url(r'^$', views.home, name='home'),
	#url(r'^$',views.index, name="index"),

]
