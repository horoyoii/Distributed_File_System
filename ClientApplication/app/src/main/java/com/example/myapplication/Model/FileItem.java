package com.example.myapplication.Model;

import android.graphics.Bitmap;

public class FileItem {

    private String file_name;
    private String file_last_update_date;
    private Bitmap file_thumbnail;
    private String file_type;

    public FileItem(String file_name, String file_last_update_date, String file_type) {
        this.file_name = file_name;
        this.file_last_update_date = file_last_update_date;
        this.file_type = file_type;
    }

    public String getFile_name() {
        return file_name;
    }

    public void setFile_name(String file_name) {
        this.file_name = file_name;
    }

    public String getFile_last_update_date() {
        return file_last_update_date;
    }

    public void setFile_last_update_date(String file_last_update_date) {
        this.file_last_update_date = file_last_update_date;
    }

    public Bitmap getFile_thumbnail() {
        return file_thumbnail;
    }

    public void setFile_thumbnail(Bitmap file_thumbnail) {
        this.file_thumbnail = file_thumbnail;
    }

    public String getFile_type() {
        return file_type;
    }

    public void setFile_type(String file_type) {
        this.file_type = file_type;
    }
}
