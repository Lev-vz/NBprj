package com.mycompany.guesscard;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import javax.imageio.ImageIO;

/*@author lev*/
public class nomen {
    ArrayList<BufferedImage> imgArr;
    int h, w;
    
    nomen(int h1, int w1) { 
        h = h1; w = w1;
        imgArr = new ArrayList<>();
        for(int i = 2; i <= 14; i++){
            try{
                File f = new File(i+".png");
                BufferedImage img = ImageIO.read(f);
                imgArr.add(img);
            }catch(IOException ex){}
        }
    }
    int compare(BufferedImage img2){
        int min = 32767;
        int cnt = 2;
        int ret = -1;
        for(BufferedImage img1: imgArr){
            int summ = 0;
            for(int i=0; i < h; i++){
                for(int j=0; j< w; j++){
                    Color c1 = new Color(img1.getRGB(j, i));
                    Color c2 = new Color(img2.getRGB(j, i));
                    int x = (c1.getGreen()>100)? 1 : 0;
                    int y = (c2.getGreen()>100)? 1 : 0;
                    //System.out.print(y+" ");
                    summ += (x!=y)? 1 : 0;
                }
                //System.out.println("");
            }
            //System.out.print(summ +", ");
            //if(summ < 150) return cnt;
            if(min > summ){
                min = summ;
                ret = cnt;
            }
            cnt++;
        }
        //System.out.println("");
        return ret;
    }
}
