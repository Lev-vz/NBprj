package com.mycompany.guesscard;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/*@author lev*/
public class Main {
    public static void main(String[] args) throws IOException {
        //System.out.println(args[0]);
        if(!args[0].equals("continue")) return;
        int wNomen = 31;
        int hNomen = 26;
        nomen n1 = new nomen(hNomen, wNomen);
        String dir ="/home/lev/Проекты/карты/1";
        File[] filesInDirectory = new File(dir).listFiles();//получаем список элементов по указанному адресу
        for (File f : filesInDirectory) {//пробегаем по списку элементов по указанному адресу
            BufferedImage img = ImageIO.read(f);
            System.out.println("f="+f.getName());
            int x = 146;
            int isWhite = 185;
            int isBlack = 176;
            for(int i = 0; i<5; i++){
                if(new Color(img.getRGB(isWhite + i*72, 600)).getGreen() < 100 ) break;
                int suit = 0;
                if(new Color(img.getRGB(isWhite + i*72, 650)).getRed() > 100 ) suit = 3;
                
                if(suit > 1) {if(new Color(img.getRGB(isWhite + i*72, 638)).getGreen() < 100 ) suit = 2;}
                else         {if(new Color(img.getRGB(isBlack + i*72, 645)).getGreen() > 100 ) suit = 1;}
                String suitS;
                switch(suit){
                    case(0): suitS = "П"; break;
                    case(1): suitS = "Т"; break;
                    case(2): suitS = "Б"; break;
                    case(3): suitS = "Ч"; break;
                    default: suitS = "?";
                }
                int nom = n1.compare(img.getSubimage(x + i*72, 590, wNomen, hNomen));
                String nomS;
                switch(nom){
                    case(11): nomS = "J"; break;
                    case(12): nomS = "Q"; break;
                    case(13): nomS = "K"; break;
                    case(14): nomS = "A"; break;
                    default: nomS = nom + "";
                }

                System.out.print(nomS+suitS+", ");
            }
            System.out.println("");
        }
    }
}
