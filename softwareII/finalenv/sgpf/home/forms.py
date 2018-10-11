from django import forms

from .models import Concept

class ConfigurationForm(forms.Form):
    isNewConcept = forms.IntegerField()
    name = forms.CharField(max_length=25)
    value = forms.DecimalField(decimal_places=2)
    period = forms.ChoiceField(choices=(
        (0, 'No Period'),
        (1, 'Daily'),
        (2, 'Biweekly'),
        (3, 'Monthly')
    ))

    isExpense = forms.BooleanField(initial=True)

class DailyInputForm(forms.Form):
    id_concepto = forms.IntegerField()
    value = forms.DecimalField(decimal_places=2)
    from_date = forms.DateField(input_formats=['%d/%m/%Y'])
