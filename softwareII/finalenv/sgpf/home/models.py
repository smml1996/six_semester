from django.db import models

# Create your models here.

class Concept(models.Model):
    name = models.CharField(max_length=30)
    value = models.DecimalField(decimal_places=2, max_digits=10)
    period = models.IntegerField()
    type = models.BooleanField(default=False)
    id_user = models.IntegerField()
    is_disabled = models.BooleanField(default=False)

class DailyInput(models.Model):
    id_user = models.IntegerField()
    id_concept = models.IntegerField()
    value = models.DecimalField(decimal_places=2, max_digits=10)
    date_from = models.DateField()
