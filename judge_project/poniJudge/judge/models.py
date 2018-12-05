from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Scores(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, default=None)
    problem1_status = models.CharField(max_length = 30)
    problem2_status = models.CharField(max_length =30)
    problem3_status = models.CharField(max_length = 30)
    score = models.DecimalField(decimal_places = 2, max_digits=10)
