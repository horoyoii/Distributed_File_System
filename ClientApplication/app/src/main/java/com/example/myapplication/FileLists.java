package com.example.myapplication;

public class FileLists {
    private String uid;
    private String name;
    private String lastupdatetime;

    public FileLists(String uid, String name, String lastupdatetime) {
        this.uid = uid;
        this.name = name;
        this.lastupdatetime = lastupdatetime;
    }

    public String getUid() {
        return uid;
    }

    public void setUid(String uid) {
        this.uid = uid;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getLastupdatetime() {
        return lastupdatetime;
    }

    public void setLastupdatetime(String lastupdatetime) {
        this.lastupdatetime = lastupdatetime;
    }
}
