{
 cout << "******************************" << endl;
 cout << "*  Welcome Evgeny to ROOT v" << gROOT->GetVersion() << "  *" << endl;
 cout << "******************************" << endl;
 cout << endl;

 gStyle->SetCanvasColor(kWhite);     // background is no longer mouse-dropping white
 gStyle->SetPalette(1,0);            // blue to red false color palette. Use 9 for b/w
 gStyle->SetCanvasBorderMode(0);     // turn off canvas borders
 gStyle->SetPadBorderMode(0);
 gStyle->SetPaintTextFormat("5.2f");  // What precision to put numbers if plotted with "TEXT"

 gStyle->SetHistLineWidth(2);
 gStyle->SetMarkerSize(1.3);
 gStyle->SetLegendBorderSize(0);

 // For publishing:
 gStyle->SetLineWidth(2);
 gStyle->SetTextSize(1.5);
 gStyle->SetLabelSize(0.06,"xyz");
 gStyle->SetTitleSize(0.06,"xyz");
 gStyle->SetTitleOffset(1.4,"x");
 gStyle->SetTitleOffset(1.4,"y");
 gStyle->SetPadTopMargin(0.2);
 gStyle->SetPadRightMargin(0.2);
 gStyle->SetPadBottomMargin(0.2);
 gStyle->SetPadLeftMargin(0.2);

 gStyle->SetNdivisions(6,"XYZ");

 gStyle->SetOptStat(11111);


}
