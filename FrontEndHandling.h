#pragma once

#include "AutoBlinds.h"
#include "CommandProcessing.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

void handleRoot()
{
    digitalWrite(led_pin, 1);
    server.send(200, "text/html", postForms);
    digitalWrite(led_pin, 0);
}

void handlePlain()
{
    if (server.method() != HTTP_POST)
    {
        digitalWrite(led_pin, 1);
        server.send(405, "text/plain", "Method Not Allowed");
        digitalWrite(led_pin, 0);
    }
    else
    {
        digitalWrite(led_pin, 1);
        server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
        digitalWrite(led_pin, 0);
    }
}

void handleForm()
{
    if (server.method() != HTTP_POST)
    {
        digitalWrite(led_pin, 1);
        server.send(405, "text/plain", "Method Not Allowed");
        digitalWrite(led_pin, 0);
    }
    else
    {
        digitalWrite(led_pin, 1);
        String message = "POST form was:\n";
        for (uint8_t i = 0; i < server.args(); i++)
        {
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        }
        server.send(200, "text/plain", message);
        digitalWrite(led_pin, 0);
    }
}

void handleFormCommand()
{
    if (server.method() != HTTP_POST)
    {
        digitalWrite(led_pin, 1);
        server.send(405, "text/plain", "Method Not Allowed");
        digitalWrite(led_pin, 0);
    }
    else
    {
        digitalWrite(led_pin, 1);
        String message = "POST form command was:\n" + server.arg("plain") + "\n";
        // Understand message
        String command = server.arg("command");
        int value = atoi(server.arg("value").c_str());

        if (command != "")
        {
            message += "Recognized command: \'" + command + "\' recognized value: \'" + value + "\'";
            processCommand(command.c_str(), value);
        }
        else
        {
            message += "Command cannot be empty";
        }
        server.send(200, "text/plain", message);
        digitalWrite(led_pin, 0);
    }
}

void handleNotFound()
{
    digitalWrite(led_pin, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led_pin, 0);
}