package com.example.myapplication;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MyFileListsActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {
    List<FileItem> rowItems;
    ListView mylistview;
    MyApplication app;

    String[] file_names;
    String[] file_update_dates;

    int cnt=0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_file_lists);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        app = (MyApplication)getApplication();

        getFileItemsFromServer();



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
                                    "txt");
                            rowItems.add(item);
                        }

                        for (int i = 0; i < data.size(); i++) {
                            Log.e("data" + i, data.get(i).getName() + "");
                        }
                    }

                    for(int i=0;i<rowItems.size();i++)
                        getFileImageFromServer(data.get(i).getName());
                }
            }

            @Override
            public void onFailure(@NonNull Call<List<FileLists>> call, @NonNull Throwable t) {

            }
        });
    }


    private void getFileImageFromServer(String filename){
        // TODO: fetchCaptcha(app.getUserID(), filename); -> 이렇게 바뀌어야 한다. 썸네일 받아올때
        Call<ResponseBody> call = ((MyApplication)getApplication()).getRetrofitExService().fetchCaptcha(app.getUserID(), "test.jpg");
        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                Log.d("HHH", "onResponse called");
                if (response.isSuccessful()) {
                    if (response.body() != null) {
                        Log.d("HHH", "success");
                        // display the image data in a ImageView or save it
                        Bitmap bmp = BitmapFactory.decodeStream(response.body().byteStream());
                        rowItems.get(cnt).setFile_thumbnail(bmp);

                    } else {
                        // TODO
                    }
                } else {
                    Log.d("HHH", "image down failed");
                    // 유효한 이미지가 없으면 디폴트 이미지를 사용한다.
                    // Bitmap _bit = BitmapFactory.decodeResource(getResources(), R.drawable.bbb);

                }

                cnt++;
                if(cnt == rowItems.size())
                    setListViewData();
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                // TODO
            }
        });
    }




    // 네트워크 응답 완료 시 호출되는 callback 함수
    /*
    1) 리스트뷰에 아이템을 추가한다.
    2) 액션바에 총 아이템 수를 추가한다.
     */
    private void setListViewData() { // 비동기적인 처리가 필요하다.
        Log.d("HHH", "called SetListView Data");
        mylistview = (ListView) findViewById(R.id.list);

        FileListAdapter adapter = new FileListAdapter(this, rowItems);

        mylistview.setAdapter(adapter);
        mylistview.setOnItemClickListener(this);

        String action_title = "모든 파일 "+rowItems.size();
        getSupportActionBar().setTitle(action_title);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        String member_name = rowItems.get(position).getFile_name();
        Toast.makeText(getApplicationContext(), "" + member_name,
                Toast.LENGTH_SHORT).show();
    }

}
