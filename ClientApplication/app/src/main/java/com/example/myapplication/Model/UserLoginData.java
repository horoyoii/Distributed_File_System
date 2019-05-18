package com.example.myapplication.Model;

public class UserLoginData {
    String token;


    UserLoginData(String token) {
        this.token = token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getToken() {
        return "token "+token;
    }
}
