from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^conf/$', views.conf, name='conf'),
	url( r'^change_percentage/$',views.changePercentage,name='changePercentage'),
	url(r'^daily_input/$', views.dailyInput, name='dailyInput'),
	url(r'^visualize/$',views.visualize, name="visualize"),
	url(r'^balance_simulator/$',views.simulateBalance, name="simulator"),
	url(r'^savings_history/$',views.visualizeSavings, name="savings"),
	url(r'^delete_concept/$', views.disableConcepts, name='deleteConcept'),
	url(r'^delete_daily/$', views.deleteDailyInput, name='deleteDailyInput'),
	url(r'^$', views.home, name='home'), # default page to be loaded

]
