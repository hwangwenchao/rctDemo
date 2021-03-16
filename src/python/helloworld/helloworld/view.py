from django.http import HttpResponse
from django.shortcuts import render

#def hello(request):
#    return HttpResponse("Hello, Old Hwang!")
def hello(request):
    context = {}
    context['hello'] = 'Hello, Old yellow!'
    return render(request, 'hello.html', context)