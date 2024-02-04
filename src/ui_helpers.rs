use binrw::{BinResult, BinWrite};
use egui::{emath, Color32, DragValue, TextEdit, TextStyle, Ui, WidgetText};

/// Helper function to write a `egui::Color32` to binwrite api
#[binrw::writer(writer, endian)]
pub fn write_color32(clr: &Color32) -> BinResult<()> {
    clr.r().write_options(writer, endian, ())?;
    clr.g().write_options(writer, endian, ())?;
    clr.b().write_options(writer, endian, ())?;
    clr.a().write_options(writer, endian, ())?;
    Ok(())
}

/// Helper function to edit a `egui::Color32` with support for text entry too
pub fn color_edit(
    ui: &mut Ui,
    color: &mut Color32,
    color_str: &mut String,
    label: impl Into<WidgetText>,
) {
    ui.horizontal(|ui| {
        ui.label(label);
        if TextEdit::singleline(color_str)
            .font(TextStyle::Monospace)
            .desired_width(0.0)
            .clip_text(false)
            .show(ui)
            .response
            .lost_focus()
        {
            match Color32::from_hex(color_str.as_str()) {
                Ok(clr) => *color = clr,
                Err(_) => *color_str = color.to_hex(),
            };
        };
        if ui.color_edit_button_srgba(color).changed() {
            *color_str = color.to_hex();
        };
    });
}

/// Helper function to edit any numerical value
pub fn num_edit<Num: emath::Numeric>(ui: &mut Ui, num: &mut Num, label: impl Into<WidgetText>) {
    ui.horizontal(|ui| {
        ui.label(label);
        ui.add(DragValue::new(num).speed(1));
    });
}

/// Helper function to edit a table of numerical values
pub fn nums_edit<Num: emath::Numeric>(ui: &mut Ui, nums: &mut [Num], label: impl Into<WidgetText>) {
    ui.horizontal(|ui| {
        ui.label(label);
        for num in nums {
            ui.add(DragValue::new(num).speed(1));
        }
    });
}
