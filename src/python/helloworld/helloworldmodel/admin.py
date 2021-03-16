from django.contrib import admin
from helloworldmodel.models import Test, Contact, Tag

admin.site.register([Test, Contact, Tag])

# Register your models here.
