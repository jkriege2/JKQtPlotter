#!/usr/bin/env python
import commands
import datetime

date = datetime.date.today().strftime("%d %b %Y")
tag = commands.getoutput("git tag").split("\n")[-1]
log = commands.getoutput("git log --reverse --pretty=format:'- %%s' %s.. sources" % tag)
version = commands.getoutput("grep -o '^VERSION=.*.$' Makefile | cut --delimiter='=' --fields=2")
print "%s (Khaled Hosny) <XITS> Version %s" %(date, version)
print log
print
