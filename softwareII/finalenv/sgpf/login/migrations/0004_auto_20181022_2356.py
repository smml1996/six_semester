# Generated by Django 2.1 on 2018-10-22 23:56

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('login', '0003_auto_20181022_2349'),
    ]

    operations = [
        migrations.AddField(
            model_name='savings',
            name='month',
            field=models.IntegerField(default=9),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='savings',
            name='year',
            field=models.IntegerField(default=2018),
            preserve_default=False,
        ),
    ]
