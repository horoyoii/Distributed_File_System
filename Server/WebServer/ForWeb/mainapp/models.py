# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey has `on_delete` set to the desired behavior.
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from django.db import models


class Fileinfo(models.Model):
    uid = models.ForeignKey('User', models.DO_NOTHING, db_column='uid', primary_key=True)
    name = models.CharField(max_length=50)
    lastupdatetime = models.CharField(max_length=50, blank=True, null=True)

    class Meta:
        managed = True
        db_table = 'fileinfo'
        unique_together = (('uid', 'name'),)


class User(models.Model):
    uid = models.AutoField(primary_key=True)
    id = models.CharField(db_column='ID', max_length=20, blank=True, null=True)  # Field name made lowercase.
    pw = models.CharField(db_column='PW', max_length=20, blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = True
        db_table = 'user'

    