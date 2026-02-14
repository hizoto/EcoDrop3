from manim import *
import numpy as np


class suchend(Scene):
    def construct(self):

        # ----------------------------
        # Rechte Wand
        # ----------------------------
        wall = Line(
            start=UP * 3 + RIGHT * 5,
            end=DOWN * 3 + RIGHT * 5,
            stroke_width=6
        )
        self.add(wall)

        # ----------------------------
        # EcoDrop
        # ----------------------------
        ecodrop = Square(side_length=1.5)
        ecodrop.set_fill(LIGHT_BROWN, opacity=0.5)
        ecodrop.move_to(LEFT * 2)
        self.play(FadeIn(ecodrop))

        # ----------------------------
        # TOF Sensoren
        # ----------------------------
        tof_front = Rectangle(width=0.2, height=0.4, color=GREEN)
        tof_back = Rectangle(width=0.2, height=0.4, color=GREEN)

        tof_front.next_to(ecodrop, RIGHT, buff=0)
        tof_front.shift(UP * 0.4)

        tof_back.next_to(ecodrop, RIGHT, buff=0)
        tof_back.shift(DOWN * 0.4)

        self.add(tof_front, tof_back)

        # ----------------------------
        # Wellenfunktion
        # ----------------------------
        def create_wave(sensor, direction="right"):

            start = sensor.get_center() + RIGHT * (sensor.width / 2)
            end_x = wall.get_left()[0]
            length = end_x - start[0]

            wave = VMobject(color=YELLOW)

            x_vals = np.linspace(0, length, 200)

            if direction == "right":
                points = [
                    start + RIGHT * x + UP * 0.1 * np.sin(15 * x)
                    for x in x_vals
                ]
            else:
                points = [
                    start + RIGHT * (length - x) + UP * 0.1 * np.sin(15 * x)
                    for x in x_vals
                ]

            wave.set_points_smoothly(points)
            return wave

        # ----------------------------
        # Ping Pong Animation
        # ----------------------------
        for _ in range(2):

            wave_f = create_wave(tof_front, "right")
            wave_b = create_wave(tof_back, "right")

            self.play(Create(wave_f), Create(wave_b), run_time=0.6)
            self.play(FadeOut(wave_f), FadeOut(wave_b), run_time=0.4)

            wave_f = create_wave(tof_front, "left")
            wave_b = create_wave(tof_back, "left")

            self.play(Create(wave_f), Create(wave_b), run_time=0.6)
            self.play(FadeOut(wave_f), FadeOut(wave_b), run_time=0.4)

        # ----------------------------
        # Messwerte
        # ----------------------------
        front_val = 98
        back_val = 102

        front_text = Text(f"{front_val} mm", font_size=28)
        back_text = Text(f"{back_val} mm", font_size=28)

        front_text.next_to(tof_front, UP)
        back_text.next_to(tof_back, DOWN)

        self.play(FadeIn(front_text), FadeIn(back_text))

        # ----------------------------
        # Obere Szene nach oben verschieben
        # ----------------------------

        upper_scene = VGroup(
            ecodrop,
            tof_front,
            tof_back,
            wall,
            front_text,
            back_text
        )

        self.play(
            upper_scene.animate.shift(UP * 1.5),
            run_time=0.8
        )


        # ----------------------------
        # CODE BLOCK unten sauber
        # ----------------------------

        font = "Consolas"  # oder "Courier New"

        line1 = Text("tiefeSchacht = 60", font=font, font_size=28)
        line2 = Text("messdifferenz = tofVorne - tofHinten", font=font, font_size=28)
        line3 = Text("while (abs(messdifferenz) < tiefeSchacht) {", font=font, font_size=28)

        # echter Einrückungs-Spacer (fixe Breite eines Zeichens)
        indent = Text("    ", font=font, font_size=28)
        indent.set_opacity(0)  # unsichtbar

        line4_text = Text("weiterSuchen()", font=font, font_size=28)
        line4 = VGroup(indent, line4_text).arrange(RIGHT, buff=0)

        line5 = Text("}", font=font, font_size=28)

        code_block = VGroup(line1, line2, line3, line4, line5)
        code_block.arrange(DOWN, aligned_edge=LEFT, buff=0.25)
        # weiter nach oben schieben
        code_block.to_edge(DOWN)
        code_block.shift(UP * 0.8)

        self.play(Write(code_block))
        self.wait(10)
