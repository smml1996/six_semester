from django.db import models

# Create your models here.

class Concept(models.Model):
    name = models.CharField(max_length=30)
    value = models.CharField(max_length=30)
    period = models.IntegerField()
    type = models.BooleanField(default=False)
    isFixed = models.IntegerField()
    id_user = models.IntegerField()
