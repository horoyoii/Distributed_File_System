package com.example.myapplication.Adapter;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.myapplication.APP.MyApplication;
import com.example.myapplication.Model.FileItem;
import com.example.myapplication.Model.UserLoginData;
import com.example.myapplication.R;

import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class FileListAdapter extends BaseAdapter {

    Context context;
    List<FileItem> rowItems;

    public FileListAdapter(Context context, List<FileItem> rowItems) {
        this.context = context;
        this.rowItems = rowItems;
    }

    @Override
    public int getCount() {
        return rowItems.size();
    }
    @Override
    public Object getItem(int position) {
        return rowItems.get(position);
    }
    @Override
    public long getItemId(int position) {
        return rowItems.indexOf(getItem(position));
    }
    /* private view holder class */
    private class ViewHolder {
        ImageView profile_pic;
        TextView member_name;
        TextView status;
        TextView contactType;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder = null;
        LayoutInflater mInflater = (LayoutInflater) context
                .getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.row_layout, null);
            holder = new ViewHolder();
            holder.member_name = (TextView) convertView
                    .findViewById(R.id.member_name);
            holder.profile_pic = (ImageView) convertView
                    .findViewById(R.id.profile_pic);

            holder.status = (TextView) convertView.findViewById(R.id.status);
            holder.contactType = (TextView) convertView
                    .findViewById(R.id.contact_type);
            FileItem row_pos = rowItems.get(position);
            //holder.profile_pic.setImageResource(row_pos.getFile_thumbnail());
            holder.profile_pic.setImageBitmap(row_pos.getFile_thumbnail());



            holder.member_name.setText(row_pos.getFile_name());
            holder.status.setText(row_pos.getFile_last_update_date());
            holder.contactType.setText(row_pos.getFile_type());
            convertView.setTag(holder);

        } else {
            holder = (ViewHolder) convertView.getTag();
        }
        return convertView;
    }
}
