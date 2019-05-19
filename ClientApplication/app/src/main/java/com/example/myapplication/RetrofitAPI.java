package com.example.myapplication;

import com.example.myapplication.Model.FileLists;
import com.example.myapplication.Model.UserLoginData;

import java.util.HashMap;
import java.util.List;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.FieldMap;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.POST;
import retrofit2.http.Path;

public interface RetrofitAPI {
    String URL = "http://115.145.241.85:8000/";
    /**
     * GET 방식, URL/posts/{userId} 호출.
     * Data Type의 JSON을 통신을 통해 받음.
     * @Path("userId") String id : id 로 들어간 STring 값을, 첫 줄에 말한
     * {userId}에 넘겨줌.
     * userId에 1이 들어가면
     * "http://jsonplaceholder.typicode.com/posts/1" 이 최종 호출 주소.
     * @param userId 요청에 필요한 userId
     * @return Data 객체를 JSON 형태로 반환.
     */

    @GET("/api/v2/users/{userId}/filelists")
    Call<List<FileLists>> getData(@Path("userId") String userId, @Header("Authorization") String JWT);



    /**
     * @Body Data param : 통신을 통해 전달하는 값이 특정 JSON 형식일 경우
     * 매번 JSON 으로 변환하지 않고, 객체를 통해서 넘겨주는 방식.
     * PUT 뿐만 아니라 다른 방식에서도 사용가능.
     * @param param 전달 데이터
     * @return Data 객체를 JSON 형태로 반환.
     */
    @POST("/obtain_token")
    Call<UserLoginData> getAuth(@Body HashMap<String, Object> params);



    /**
     * GET 방식, URL/posts/{userId} 호출.
     * Data Type의 여러 개의 JSON을 통신을 통해 받음.
     * @Query("userId") String id : getData와 다르게 뒤에 붙는 파라미터가 없음.
     * 방식은 위와 같음.
     * 단, 주소값이 "http://jsonplaceholder.typicode.com/posts?userId=1" 이 됨.
     * @param userId 요청에 필요한 userId
     * @return 다수의 Data 객체를 JSON 형태로 반환.
     */




}
