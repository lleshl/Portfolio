package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.Toast;


import java.io.File;

public class MainActivity extends AppCompatActivity {
    MyCanvas myCanvas;
    CheckBox c;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setTitle("GrapicEditer");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        myCanvas = (MyCanvas) findViewById(R.id.canv);
        c = (CheckBox) findViewById(R.id.stamp);
        c.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                myCanvas.setStamp(isChecked);
            }
        });
        int permissioninfo = ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permissioninfo == PackageManager.PERMISSION_GRANTED) {
            File file = new File(getFilesDir() + "canvas");
            if (!file.exists()) {
                file.mkdir();
                String msg = "디렉터리 생성";
                if (file.isDirectory() == false) msg = "디렉터리 생성 오류";
                Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
            }
        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0,1,0,"블러링");
        menu.findItem(1).setCheckable(true);
        menu.add(0,2,0,"엠보싱");
        menu.findItem(2).setCheckable(true);
        menu.add(0,3,0,"굵은 선");
        menu.findItem(3).setCheckable(true);
        menu.add(0,4,0,"지우개");
        menu.findItem(4).setCheckable(true);

        SubMenu subMenu = menu.addSubMenu("색상 변경 >>");
        subMenu.add(0,5,0,"검정");
        subMenu.add(0,6,0,"빨강");
        subMenu.add(0,7,0,"초록");
        subMenu.add(0,8,0,"파랑");
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch(item.getItemId()){
            case 1:
                myCanvas.setBlurring(!item.isChecked());
                item.setChecked(!item.isChecked());
                return true;
            case 2:
                myCanvas.setEmbossing(!item.isChecked());
                item.setChecked(!item.isChecked());
                return true;
            case 3:
                myCanvas.setB(!item.isChecked());
                item.setChecked(!item.isChecked());
                return true;
            case 4:
                myCanvas.setEraser(!item.isChecked());
                item.setChecked(!item.isChecked());
                return true;
            case 5:
                myCanvas.setPenColor(MyCanvas.pencolor.black);
                return true;
            case 6:
                myCanvas.setPenColor(MyCanvas.pencolor.red);
                return true;
            case 7:
                myCanvas.setPenColor(MyCanvas.pencolor.green);
                return true;
            case 8:
                myCanvas.setPenColor(MyCanvas.pencolor.blue);
                return true;
    }
        return super.onOptionsItemSelected(item);
}
    public void onClick(View v){
        switch (v.getId()){
            case R.id.erase:
                myCanvas.clear();
                break;
            case R.id.open:
                if (!myCanvas.Load(getFilesDir() + "canvas/"+
                        "canvas.jpg"))
                    Toast.makeText(getApplicationContext(),
                            "불러오기 실패",Toast.LENGTH_SHORT).show();
                break;
            case R.id.save:
                if (myCanvas.Save(getFilesDir() + "canvas/"+
                        "canvas.jpg"))
                    Toast.makeText(getApplicationContext(),
                            "저장 성공",Toast.LENGTH_SHORT).show();
                else Toast.makeText(getApplicationContext(),
                        "저장 실패",Toast.LENGTH_SHORT).show();
                break;
            default:
                c.setChecked(true);
                myCanvas.setStamp(true);
                myCanvas.setOptType((String) v.getTag());
        }
    }
}
