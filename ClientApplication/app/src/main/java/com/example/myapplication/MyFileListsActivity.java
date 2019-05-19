package com.example.myapplication;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import com.example.myapplication.APP.MyApplication;
import com.example.myapplication.Adapter.FileListAdapter;
import com.example.myapplication.Model.FileItem;
import com.example.myapplication.Model.FileLists;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MyFileListsActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {
    List<FileItem> rowItems;
    ListView mylistview;
    MyApplication app;

    String[] file_names;
    String[] file_update_dates;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_file_lists);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        app = (MyApplication)getApplication();

        getFileItemsFromServer();
        //setListViewData();




        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
    }

    private void getFileItemsFromServer(){
        Log.d("HHH", "called getFIle");
        app.getRetrofitExService().getData(app.getUserID(), app.getJWT()).enqueue(new Callback<List<FileLists>>() {
            @Override
            public void onResponse(@NonNull Call<List<FileLists>> call, @NonNull Response<List<FileLists>> response) {
                if (response.isSuccessful()) {
                    List<FileLists> data = response.body();
                    if (data != null) {
                        rowItems = new ArrayList<>();
                        for (int i = 0; i < data.size(); i++) {
                            FileItem item = new FileItem(data.get(i).getName(),
                                    data.get(i).getLastupdatetime(),
                                    R.drawable.cap,
                                    "txt");
                            rowItems.add(item);
                        }

                        String result="";
                        for (int i = 0; i < data.size(); i++) {
                            Log.e("data" + i, data.get(i).getName() + "");
                        }
                    }

                    setListViewData();
                }
            }

            @Override
            public void onFailure(@NonNull Call<List<FileLists>> call, @NonNull Throwable t) {

            }
        });
    }

    // 네트워크 응답 완료 시 호출되는 callback 함수
    private void setListViewData() { // 비동기적인 처리가 필요하다.
        Log.d("HHH", "called SetListView Data");
        mylistview = (ListView) findViewById(R.id.list);

        FileListAdapter adapter = new FileListAdapter(this, rowItems);
        mylistview.setAdapter(adapter);
        mylistview.setOnItemClickListener(this);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        String member_name = rowItems.get(position).getFile_name();
        Toast.makeText(getApplicationContext(), "" + member_name,
                Toast.LENGTH_SHORT).show();
    }

}
