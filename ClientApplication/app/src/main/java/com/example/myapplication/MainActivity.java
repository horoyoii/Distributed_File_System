package com.example.myapplication;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.JsonReader;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    EditText uidEditText;
    TextView resultTextView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        uidEditText = findViewById(R.id.uid);
        resultTextView = findViewById(R.id.result);

        getJSON();

    }

    public void  getJSON() {
        Thread thread = new Thread(new Runnable() {
            public void run() {
                try{

                    String url = "http://115.145.244.191:8000/api/v2/fileinfolist/"+uidEditText.getText().toString()+"/?format=json";
                    Log.d("HHH", url);

                    URL endpoint = new URL(url);
                    HttpURLConnection myConnection = (HttpURLConnection) endpoint.openConnection();
                    /*
                    어플리케이션을 고유하게 식별하기 위하여 추가적인 작업이 필요할 것이다.
                    웹 서버 입장에서 이것이 android 클라이언트로부터 온 요청이다. 등등
                    https://code.tutsplus.com/ko/tutorials/android-from-scratch-using-rest-apis--cms-27117

                     */

                    myConnection.setReadTimeout(3000);
                    myConnection.setConnectTimeout(3000);
                    myConnection.setDoOutput(true);
                    myConnection.setDoInput(true);
                    myConnection.setRequestMethod("GET");
                    myConnection.setUseCaches(false);
                    myConnection.connect();

                    if (myConnection.getResponseCode() == 200) {
                        Log.d("HHH", "response success");
                        // Further processing here
                        InputStream responseBody = myConnection.getInputStream();
                        InputStreamReader inputStreamReader =
                                new InputStreamReader(responseBody, "UTF-8");

                        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                        StringBuilder sb = new StringBuilder();
                        String line;


                        while ((line = bufferedReader.readLine()) != null) {
                            sb.append(line);
                        }

                        bufferedReader.close();
                        myConnection.disconnect();

                        String response = sb.toString().trim();
                        Log.d("HHH", response);


                    } else {
                        // Error handling code goes here
                    }
                }catch (Exception e){
                    Log.d("HHH",e.toString());
                }

            }

        });
        thread.start();


    }












    public void SendRequest(View v){
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                try{

                    String url = "http://115.145.244.191:8000/api/v2/fileinfolist/"+uidEditText.getText().toString()+"/?format=json";
                    Log.d("HHH", url);

                    URL endpoint = new URL(url);
                    HttpURLConnection myConnection = (HttpURLConnection) endpoint.openConnection();
                    /*
                    어플리케이션을 고유하게 식별하기 위하여 추가적인 작업이 필요할 것이다.
                    웹 서버 입장에서 이것이 android 클라이언트로부터 온 요청이다. 등등
                    https://code.tutsplus.com/ko/tutorials/android-from-scratch-using-rest-apis--cms-27117

                     */

                    myConnection.setReadTimeout(3000);
                    myConnection.setConnectTimeout(3000);
                    myConnection.setDoOutput(true);
                    myConnection.setDoInput(true);
                    myConnection.setRequestMethod("GET");
                    myConnection.setUseCaches(false);
                    myConnection.connect();

                    if (myConnection.getResponseCode() == 200) {
                        Log.d("HHH", "response success");
                        // Further processing here
                        InputStream responseBody = myConnection.getInputStream();
                        InputStreamReader inputStreamReader =
                                new InputStreamReader(responseBody, "UTF-8");

                        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                        StringBuilder sb = new StringBuilder();
                        String line;


                        while ((line = bufferedReader.readLine()) != null) {
                            sb.append(line);
                        }

                        bufferedReader.close();
                        myConnection.disconnect();

                        String response = sb.toString().trim();
                        Log.d("HHH", response);


                    } else {
                        // Error handling code goes here
                    }
                }catch (Exception e){
                    Log.d("HHH",e.toString());
                }


            }
        });
    }



}
