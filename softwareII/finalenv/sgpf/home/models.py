from django.db import models

# Create your models here.

#all models have attribute id, column created by django
# all models have as default primary key attribute id

class Concept(models.Model):
    name = models.CharField(max_length=30)
        # concept's name
    value = models.DecimalField(decimal_places=2, max_digits=10)
        # monetary value
    period = models.IntegerField()
        # 0: no period
        # 1: daily
        # 2: Biweekly
        # 3: Monthly
    type = models.BooleanField(default=False)
        # expense: True
        # income: False
    id_user = models.IntegerField() #foreign key, user who created this concept
    is_disabled = models.BooleanField(default=False)

class DailyInput(models.Model):
    id_user = models.IntegerField() # foreign key pointing id at User's table
    id_concept = models.IntegerField() # foreign key pointing id at Concept's table
    value = models.DecimalField(decimal_places=2, max_digits=10) # monetary value
        # monetary value
    date_from = models.DateField()
        # date from meant to be applied this expense/income
