# cs411-T4
Group Project: Real News

# Tools
C++ 11
Qt5 Framework and IDE
newsapi.org

# Group Members
Ana,
Andrew,
Brandon,
Brian,
Michael,
Nick,
Tom,

# How-To
the .pro file can be opened and run in the Qt creator

# SRS
1. Introduction

1.1 Purpose
Stores news api information on an AWS hosted database. Clients then pull information from the AWS hosted database and displaying it on command line.

1.2 Background
Users need to view current news from all sources sorted by category of news such as “Business” or “Entertainment”.

1.2 Definitions, Acronyms and Abbreviations
AWS - Amazon Web Services
DB - Database
SQL -  Structured Query Language
API - Application Programing Interface

2. System Requirements

2.1 Actor(s) Museum Registrar(s)
Users request news articles from the database.

2.2 High level Flow
News stories are taken from newsapi and added to the DB as table entries, each news category has its own table. Clients pull selected news table from the DB and display it locally.

2.3 Basic use cases
High Priority:
Client wants news
News is grabbed from database and displayed.
Client wants to filter news
System searches news through the database and grabs the specified news and displays it.
Client wants trending news
Trending news is grabbed from database and displayed
Client wants to filter news between certain dates.
The user checks the “limi date” box and select the desired dates
Mid Priority:
Client wants to save news locally
News is saved object with link is saved locally
Low Priority:
Client wants to share news to another client
System sends news from one client to another
