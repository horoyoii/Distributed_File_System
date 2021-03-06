package com.example.myapplication;

import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;

import com.example.myapplication.APP.MyApplication;
import com.example.myapplication.Model.FileLists;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {
    EditText uidEditText;
    TextView resultTextView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        uidEditText = findViewById(R.id.uid);
        resultTextView = findViewById(R.id.result);

        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(RetrofitAPI.URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        RetrofitAPI retrofitExService = retrofit.create(RetrofitAPI.class);

        MyApplication app = (MyApplication)getApplication();

        retrofitExService.getData(app.getUserID(), app.getJWT()).enqueue(new Callback<List<FileLists>>() {
            @Override
            public void onResponse(@NonNull Call<List<FileLists>> call, @NonNull Response<List<FileLists>> response) {
                if (response.isSuccessful()) {
                    List<FileLists> datas = response.body();
                    if (datas != null) {
                        String result="";
                        for (int i = 0; i < datas.size(); i++) {
                            Log.e("data" + i, datas.get(i).getName() + "");
                            result+=datas.get(i).getName()+"\n";
                        }
                        Log.e("getData2 end", "======================================");
                        resultTextView.setText(result);
                    }
                }
            }

            @Override
            public void onFailure(@NonNull Call<List<FileLists>> call, @NonNull Throwable t) {

            }
        });




    }


}
