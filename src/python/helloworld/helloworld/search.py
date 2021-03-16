# -*- coding:utf-8 -*-

from django.http import HttpResponse
from django.shortcuts import render_to_response

def search_from(request):
    return render_to_response('search_from.html')

def search(request):
    request.encoding='utf-8'
    if 'q' in request.GET and request.GET['q']:
        message = 'your search context: ' + request.GET['q']
    else:
        message = 'blank form'
    return HttpResponse(message)
