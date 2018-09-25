from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^conf/$', views.conf, name='conf'),
	url(r'^daily_input/$', views.dailyInput, name='dailyInput'),
	url(r'^viewExpense/$',views.viewExpenses, name="visExpense"),
	url(r'^balance_simulator/$',views.balanceSimulator, name="simulator"),
	url(r'^savings_history/$',views.savingHistory, name="savings"),
	url(r'^$', views.home, name='home'),

]
