#include "button.hpp"
#include "../yg/overlay_renderer.hpp"

namespace gui
{
  Button::Button(Params const & p) : Element(p)
  {
    TextView::Params tp;

    tp.m_depth = p.m_depth + 1;
    tp.m_pivot = p.m_pivot;
    tp.m_position = yg::EPosCenter;
    tp.m_text = p.m_text;

    m_textView.reset(new TextView(tp));

    setMinWidth(p.m_minWidth);
    setMinHeight(p.m_minHeight);
    setText(p.m_text);

    setFont(EActive, yg::FontDesc(12, yg::Color(0, 0, 0, 255)));
    setFont(EPressed, yg::FontDesc(12, yg::Color(0, 0, 0, 255)));

    setColor(EActive, yg::Color(yg::Color(192, 192, 192, 255)));
    setColor(EPressed, yg::Color(yg::Color(64, 64, 64, 255)));
  }

  void Button::setOnClickListener(TOnClickListener const & l)
  {
    m_OnClickListener = l;
  }

  bool Button::onTapStarted(m2::PointD const & pt)
  {
    setState(EPressed);
    invalidate();
    return false;
  }

  bool Button::onTapCancelled(m2::PointD const & pt)
  {
    setState(EActive);
    invalidate();
    return false;
  }

  bool Button::onTapEnded(m2::PointD const & pt)
  {
    setState(EActive);
    if (m_OnClickListener)
      m_OnClickListener(this);
    invalidate();
    return false;
  }

  bool Button::onTapMoved(m2::PointD const & pt)
  {
    invalidate();
    return false;
  }

  void Button::setText(string const & text)
  {
    m_textView->setText(text);
  }

  string const & Button::text() const
  {
    return m_textView->text();
  }

  void Button::setMinWidth(unsigned minWidth)
  {
    m_minWidth = minWidth;
    invalidate();
  }

  unsigned Button::minWidth() const
  {
    return m_minWidth;
  }

  void Button::setMinHeight(unsigned minHeight)
  {
    m_minHeight = minHeight;
    invalidate();
  }

  unsigned Button::minHeight() const
  {
    return m_minHeight;
  }

  void Button::setController(Controller *controller)
  {
    m_textView->setController(controller);
    Element::setController(controller);
  }

  vector<m2::AnyRectD> const & Button::boundRects() const
  {
    if (isDirtyRect())
    {
      m_boundRects.clear();
      double k = visualScale();

      m2::RectD tr(m_textView->roughBoundRect());
      m2::RectD rc(0, 0, tr.SizeX(), tr.SizeY());

      rc.Inflate(20 * k, 10 * k);

      double dx = 0;
      double dy = 0;

      if (rc.SizeX() < m_minWidth * k)
        dx = (m_minWidth * k - rc.SizeX()) / 2;
      if (rc.SizeY() < m_minHeight * k)
        dy = (m_minHeight * k - rc.SizeY()) / 2;

      rc.Inflate(dx, dy);
      rc.Offset(-rc.minX(), -rc.minY());

      rc.Offset(tieRect(rc, math::Identity<double, 3>()));
      m_boundRects.push_back(m2::AnyRectD(rc));
      setIsDirtyRect(false);
    }

    return m_boundRects;
  }

  void Button::draw(yg::gl::OverlayRenderer * r, math::Matrix<double, 3, 3> const & m) const
  {
    if (!isVisible())
      return;

    double k = visualScale();

    r->drawRoundedRectangle(roughBoundRect(), 10 * k, color(state()), depth());

    yg::FontDesc desc = font(state());
    desc.m_size *= k;

    m_textView->draw(r, m);
  }

  void Button::setPivot(m2::PointD const &pv)
  {
    m_textView->setPivot(pv);
    Element::setPivot(pv);
  }

  void Button::setFont(EState state, yg::FontDesc const & font)
  {
    m_textView->setFont(state, font);
    Element::setFont(state, font);
  }

  void Button::setColor(EState state, yg::Color const & c)
  {
    m_textView->setColor(state, c);
    Element::setColor(state, c);
  }
}
