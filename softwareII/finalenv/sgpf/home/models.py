from django.db import models
from django.contrib.auth.models import User
# Create your models here.

#all models have attribute id, column created by django
# all models have as default primary key attribute id

class ConceptManager(models.Manager):
    def get_by_natural_key(self, name, id):
        return self.get(name=name, id=id);

class Concept(models.Model):
    objects = ConceptManager()
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
    user = models.ForeignKey(User, on_delete=models.CASCADE, default=None) #foreign key, user who created this concept
    is_disabled = models.BooleanField(default=False)

    def natural_key(self):
        return (self.name, self.id)    

    class Meta:
        unique_together = (('name', 'id'))

class DailyInput(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE) # foreign key pointing id at User's table
    concept = models.ForeignKey(Concept, on_delete=models.CASCADE) # foreign key pointing id at Concept's table
    value = models.DecimalField(decimal_places=2, max_digits=10) # monetary value
    date_from = models.DateField() # date from meant to be applied this expense/income
    savings_value = models.DecimalField(decimal_places=2, max_digits=10) #money for savings
