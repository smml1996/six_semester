from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^$', views.index, name='index'),
	url(r'^leaderboard/$', views.Leaderboard, name='leaderboard'),
	url(r'^problem/(?P<value>\d+)/$', views.problemView, name='problem'),
]
