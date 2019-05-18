package com.example.myapplication.APP;

import android.app.Application;

import com.example.myapplication.RetrofitAPI;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.converter.scalars.ScalarsConverterFactory;

public class MyApplication extends Application {
    private Retrofit retrofit;
    private RetrofitAPI retrofitExService;
    private String JWT;
    private String UserID;

    @Override
    public void onCreate() {
        super.onCreate();

        retrofit = new Retrofit.Builder()
                .baseUrl(RetrofitAPI.URL)
                .addConverterFactory(ScalarsConverterFactory.create())
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        retrofitExService = retrofit.create(RetrofitAPI.class);

    }

    public RetrofitAPI getRetrofitExService(){
        return retrofitExService;
    }

    public String getJWT() {
        return JWT;
    }

    public void setJWT(String JWT) {
        this.JWT = JWT;
    }

    public String getUserID() {
        return UserID;
    }

    public void setUserID(String userID) {
        UserID = userID;
    }
}
