from django.db import models

# Create your models here.

class Savings_Percentage(models.Model):
    id_user = models.IntegerField() # foreign key
    percentage = models.DecimalField(decimal_places=2, default=0.1, max_digits=3) # by default user is saving 10% of her/his incomes

class Savings(models.Model):
    id_user = models.IntegerField() # foreign key
    value = models.DecimalField(decimal_places=2, default=0.0, max_digits=10)
    #the following attributes are used for the savings history view
    month = models.IntegerField()
        #some integer between 1 and 12
    year = models.IntegerField()
