# Generated by Django 4.1.9 on 2023-06-23 19:59

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('core', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='device',
            name='mac',
            field=models.CharField(max_length=30, verbose_name='MAC'),
        ),
    ]
